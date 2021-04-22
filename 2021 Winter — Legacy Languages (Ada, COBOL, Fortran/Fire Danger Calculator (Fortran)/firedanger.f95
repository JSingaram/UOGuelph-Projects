!     Written by James Singaram, 0758172 on Jan. 30, 2021
!     Program FIRE takes user input data, calculates the fire risk index,
!     and outputs it to the user.
     
      PROGRAM FIRE
     
!     Variable declarations
      IMPLICIT NONE
      REAL dry,wet,isnow,precip,wind,bui,iherb, &
           df,ffm,adfm,grass,timber,fload

!     Input of variables
      CALL GET_INPUT(dry, wet, isnow, wind, bui, iherb, precip)

      CALL DANGER(dry,wet,isnow,precip,wind,bui,iherb, &
                  df,ffm,adfm,grass,timber,fload)

!     Output of calculated info from DANGER
!      CALL OUTPUT_RESULTS(dry, wet, isnow, wind, bui, iherb, precip, ffm, &
!                          adfm, grass, timber, fload)
      CALL OUTPUT_RESULTS(ffm, adfm, grass, timber, fload)

      CONTAINS


!     Gets values from user
      SUBROUTINE GET_INPUT(Dry, Wet, ISnow, Wind, Bui, IHerb, Precip)
      IMPLICIT NONE
      REAL, INTENT(OUT) :: Dry, Wet, ISnow, Wind, Bui, IHerb, Precip
      Dry = GET_DRY()
      Wet = GET_WET()
      ISnow = GET_ISNOW()
      Wind = GET_WIND()
      Bui = GET_BUI()
      IHerb = GET_IHERB()
      Precip = GET_PRECIP()
      RETURN
      END SUBROUTINE GET_INPUT


!     Get dry temperature from user
      REAL FUNCTION GET_DRY()
      IMPLICIT NONE
      WRITE (*,*) 'Enter dry temperature: '
      READ (*,*) GET_DRY
      END FUNCTION GET_DRY


!     Get wet temperature from user
      REAL FUNCTION GET_WET()
      IMPLICIT NONE
      WRITE (*,*) 'Enter wet temperature: '
      READ (*,*) GET_WET
      END FUNCTION GET_WET


!     Get snow state from user
      REAL FUNCTION GET_ISNOW()
      IMPLICIT NONE
      WRITE (*,*) 'Is there snow: '
      READ (*,*) GET_ISNOW
      END FUNCTION GET_ISNOW


!     Get wind speed from user
      REAL FUNCTION GET_WIND()
      IMPLICIT NONE
      WRITE (*,*) 'Enter wind speed: '
      READ (*,*) GET_WIND
      END FUNCTION GET_WIND


!     Get build up index from user
      REAL FUNCTION GET_BUI()
      IMPLICIT NONE
      WRITE (*,*) 'Enter build up index: '
      READ (*,*) GET_BUI
      END FUNCTION GET_BUI


!     Get herb state from user
      REAL FUNCTION GET_IHERB()
      IMPLICIT NONE
      WRITE (*,*) 'Enter herb state: '
      READ (*,*) GET_IHERB
      END FUNCTION GET_IHERB


!     Get precipitation amount from user
      REAL FUNCTION GET_PRECIP()
      IMPLICIT NONE
      WRITE (*,*) 'Enter precipitation: '
      READ (*,*) GET_PRECIP
      END FUNCTION GET_PRECIP


!     Outputs results to user
      SUBROUTINE OUTPUT_RESULTS(Ffm, Adfm, Grass, Timber, FLoad)
      IMPLICIT NONE
      REAL, INTENT(IN) :: Ffm, Adfm, Grass, Timber, FLoad
!     WRITE (*,*) 'Dry:    ', Dry
!     WRITE (*,*) 'Wet:    ', Wet
!     WRITE (*,*) 'ISnow:  ', ISnow
!     WRITE (*,*) 'Wind:   ', Wind
!     WRITE (*,*) 'Bui:    ', Bui
!     WRITE (*,*) 'IHerb:  ', IHerb
!     WRITE (*,*) 'Precip: ', Precip

      WRITE (*,*) 'Fine Fuel Moisture:     ', Ffm
      WRITE (*,*) 'Adjusted Fuel Moisture: ', Adfm
      WRITE (*,*) 'Fine Fuel Spread:       ', Grass
      WRITE (*,*) 'Timber Spread index:    ', Timber
      WRITE (*,*) 'Fire Load Index:        ', FLoad
      WRITE (*,*) 'Build Up Index:         ', Bui
      RETURN
      END SUBROUTINE OUTPUT_RESULTS



!     Calculates fire danger rating index
      SUBROUTINE DANGER(DRY,WET,ISNOW,PRECIP,WIND,BUI,IHERB, &
                        DF,FFM,ADFM,GRASS,TIMBER,FLOAD)
      IMPLICIT NONE
      REAL, INTENT(INOUT) :: DRY,WET,ISNOW,PRECIP,WIND,BUI,IHERB, &
                         DF,FFM,ADFM,GRASS,TIMBER,FLOAD
      REAL :: A(4), B(4), C(3), D(6)
      INTEGER :: I
      CALL DEFAULT_VALUES(FFM, ADFM, DF, FLOAD, GRASS, TIMBER)
      CALL POPULATE_MATH_TABLE(A, B, C, D)

      IF (ISNOW .GT. 0) THEN
          CALL SNOW_PRESENT(GRASS, TIMBER, PRECIP, BUI)
          RETURN
      END IF
      CALL CALCULATE_FFM(FFM, A, B, C, DRY, WET, I)
      CALL CALCULATE_DF(FFM, D, I, DF)
      CALL ADJUST_FFM(FFM, IHERB)
      CALL CALCULATE_BUI(BUI, PRECIP, DF)
      CALL CALCULATE_ADFM(ADFM, FFM, BUI, GRASS, TIMBER)
      IF (FFM .GE. 30.) THEN
          RETURN
      END IF
      CALL CALCULATE_GRASS_AND_TIMBER(WIND, GRASS, TIMBER, ADFM, FFM)
      IF ((TIMBER .LE. 0) .OR. (BUI .LE. 0)) THEN
          RETURN
      END IF
      CALL CALCULATE_FLOAD(FLOAD, TIMBER, BUI)

      END SUBROUTINE DANGER


!     Calculates fire load index
      SUBROUTINE CALCULATE_FLOAD(FL_FLOAD, FL_TIMBER, FL_BUI)
      IMPLICIT NONE
      REAL, INTENT(IN) :: FL_TIMBER, FL_BUI
      REAL, INTENT(OUT) :: FL_FLOAD

      FL_FLOAD = 1.75 * ALOG10(FL_TIMBER) + .32 * ALOG10(FL_BUI) - 1.640
      IF (FL_FLOAD .LE. 0.) THEN
          FL_FLOAD = 0.
      END IF
      FL_FLOAD = 10. ** FL_FLOAD
      END SUBROUTINE CALCULATE_FLOAD


!     Calculates grass and timber indexes
      SUBROUTINE CALCULATE_GRASS_AND_TIMBER(GT_WIND, GT_GRASS, GT_TIMBER, &
                                            GT_ADFM, GT_FFM)
      IMPLICIT NONE
      REAL, INTENT(IN) :: GT_WIND, GT_ADFM, GT_FFM
      REAL, INTENT(OUT) :: GT_GRASS, GT_TIMBER

      IF (GT_WIND .GE. 14.) THEN
          IF (GT_ADFM .LT. 30.) THEN
              GT_TIMBER = .00918*(GT_WIND+14.) * (33.-GT_ADFM)**1.65 - 3.
          END IF
          GT_GRASS  = .00918*(GT_WIND+14.) * (33.-GT_FFM)**1.65 - 3.
          IF (GT_GRASS .GT. 99.) THEN
              GT_GRASS = 99.
          END IF
          IF (GT_TIMBER .GT. 99.) THEN
              GT_TIMBER = 99.
          END IF
      ELSE
          IF (GT_ADFM .LT. 30.) THEN
              GT_TIMBER = .01312*(GT_WIND+6.) * (33.-GT_ADFM)**1.65 - 3.
          END IF
          GT_GRASS = .01312*(GT_WIND+6.) * (33.-GT_FFM)**1.65 - 3.
          IF (GT_GRASS .LT. 1.) THEN
              GT_GRASS = 1.
          END IF
          IF (GT_TIMBER .LT. 1.) THEN
              GT_TIMBER = 1.
          END IF
      END IF
      END SUBROUTINE CALCULATE_GRASS_AND_TIMBER


!     Calculates adjusted fuel moisture
      SUBROUTINE CALCULATE_ADFM(ADFM_ADFM, ADFM_FFM, ADFM_BUI, ADFM_GRASS, &
                                ADFM_TIMBER)
      IMPLICIT NONE
      REAL, INTENT(IN) :: ADFM_FFM, ADFM_BUI
      REAL, INTENT(OUT) :: ADFM_ADFM, ADFM_GRASS, ADFM_TIMBER

      ADFM_ADFM = .9 * ADFM_FFM + .5 + 9.5 * EXP(-ADFM_BUI / 50.)
      IF (ADFM_ADFM .GE. 30.) THEN
          IF (ADFM_FFM .GE. 30.) THEN
              ADFM_GRASS = 1.
          END IF
          ADFM_TIMBER = 1.
      END IF
      END SUBROUTINE CALCULATE_ADFM


!     Calculates build up index
      SUBROUTINE CALCULATE_BUI(BUI_BUI, BUI_PRECIP, BUI_DF)
      IMPLICIT NONE
      REAL, INTENT (IN) :: BUI_PRECIP, BUI_DF
      REAL, INTENT (OUT) :: BUI_BUI

      IF (BUI_PRECIP .GT. 0.1) THEN 
          BUI_BUI= - 50. * ALOG(1. - (1. - EXP(-BUI_BUI / 50.)) * &
                   EXP(-1.175 * (BUI_PRECIP - .1)))
          IF (BUI_BUI .LT. 0) THEN
              BUI_BUI = 0.
          END IF
      END IF
      
      BUI_BUI = BUI_BUI + BUI_DF

      END SUBROUTINE CALCULATE_BUI


!     Adjusts fine fuel moisture based on herb stage
      SUBROUTINE ADJUST_FFM(A_FFM, A_IHERB)
      IMPLICIT NONE
      REAL, INTENT(IN) :: A_IHERB
      REAL, INTENT(OUT) :: A_FFM

      IF (A_FFM .LT. 1.) THEN
          A_FFM = 1.
      END IF
      A_FFM = A_FFM + ( A_IHERB-1 ) * 5.
      END SUBROUTINE ADJUST_FFM


!     Calculates drying factor based on fine fuel moisture
      SUBROUTINE CALCULATE_DF(DF_FFM, DF_D, DF_I, DF_DF)
      IMPLICIT NONE
      REAL, INTENT(IN) :: DF_FFM, DF_D(6)
      REAL, INTENT(OUT) :: DF_DF
      INTEGER, INTENT(OUT) :: DF_I

      DO DF_I=1, 6
          IF ((DF_FFM - DF_D(DF_I)) .GT. 0) THEN
              DF_DF = DF_I - 1
              EXIT
          ELSE
              DF_DF = 7
          END IF
      END DO
      END SUBROUTINE CALCULATE_DF


!     Calculates FFM based on dry and wet temperatures
      SUBROUTINE CALCULATE_FFM(FFM_FFM, FFM_A, FFM_B, FFM_C, FFM_DRY, &
                               FFM_WET, FFM_I)
      IMPLICIT NONE
      REAL, INTENT(IN) :: FFM_A(4), FFM_B(4), FFM_C(3), FFM_DRY, FFM_WET
      INTEGER, INTENT(INOUT) :: FFM_I
      REAL, INTENT(OUT) :: FFM_FFM

      DO FFM_I=1, 3
          IF ((FFM_DRY - FFM_WET) .LT. FFM_C(FFM_I)) THEN
              FFM_FFM = FFM_B(FFM_I) * EXP(FFM_A(FFM_I) * (FFM_DRY - FFM_WET))
              EXIT
          ELSE
              FFM_FFM = FFM_B(4) * EXP(FFM_A(FFM_I) * (FFM_DRY - FFM_WET))
          END IF
      END DO
      END SUBROUTINE CALCULATE_FFM


!     Calculates spread and build up indexes when snow present
      SUBROUTINE SNOW_PRESENT(SNOW_GRASS, SNOW_TIMBER, SNOW_PRECIP, SNOW_BUI)
      IMPLICIT NONE
      REAL, INTENT(IN) :: SNOW_PRECIP
      REAL, INTENT(OUT) :: SNOW_GRASS, SNOW_TIMBER, SNOW_BUI
      SNOW_GRASS = 0
      SNOW_TIMBER = 0

      IF (SNOW_PRECIP .GT. 0.1) THEN
          SNOW_BUI=-50.*ALOG(1.-(1.-EXP(-SNOW_BUI/50.)) &
                   *EXP( -1.175*(SNOW_PRECIP-.1)))
      END IF
      IF (SNOW_BUI .LT. 0) THEN
          SNOW_BUI = 0
      END IF
      END SUBROUTINE SNOW_PRESENT


!     Initializes starting values for DANGER
      SUBROUTINE DEFAULT_VALUES(FFM, ADFM, DF, FLOAD, GRASS, TIMBER)
      IMPLICIT NONE
      REAL, INTENT(OUT) :: FFM, ADFM, DF, FLOAD, GRASS, TIMBER
      FFM= 99.
      ADFM= 99.
      GRASS = 99.
      TIMBER = 99.
      DF = 0.
      FLOAD = 0.
      END SUBROUTINE DEFAULT_VALUES


!     Defines values stored in table used to compute danger ratings
      SUBROUTINE POPULATE_MATH_TABLE(a, b, c, d)
      IMPLICIT NONE
      REAL, INTENT(OUT) :: a(4), b(4), c(3), d(6)
      a(1) = -0.185900
      a(2) = -0.85900
      a(3) = -0.059660
      a(4) = -0.077373
      b(1) = 30.0
      b(2) = 19.2
      b(3) = 13.8
      b(4) = 22.5
      c(1) = 4.5
      c(2) = 12.5
      c(3) = 27.5
      d(1) = 16.0
      d(2) = 10.0
      d(3) = 7.0
      d(4) = 5.0
      d(5) = 4.0
      d(6) = 3.0
      END SUBROUTINE POPULATE_MATH_TABLE


      END PROGRAM FIRE